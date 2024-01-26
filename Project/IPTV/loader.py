import asyncio
import sys

from aiogram import Bot, Dispatcher, types
from aiogram.contrib.fsm_storage.redis import RedisStorage2
import sqlalchemy as db
from sqlalchemy.orm import sessionmaker

from data import config

from utils.misc.db_session import DBSession

if sys.platform in ["win32", "win64"]:
    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

loop = asyncio.new_event_loop()
bot = Bot(token=config.TELEGRAM_BOT_TOKEN, parse_mode=types.ParseMode.HTML, loop=loop)
storage = RedisStorage2(config.REDIS_HOST, config.REDIS_PORT, db=config.REDIS_DB)
engine = db.create_engine('sqlite:///iptv.db')
# payment_system = [Yoomoney()]
session_factory = sessionmaker(bind=engine)
db_session = DBSession(session_factory())
dp = Dispatcher(bot, storage=storage, run_tasks_by_default=True)
