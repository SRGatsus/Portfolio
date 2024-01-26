from aiogram import Dispatcher
from aiogram.dispatcher.filters import CommandStart, CommandHelp
from .help import bot_help
from .start import start_handler
from .user_info import user_info
from ...states.user import UserStates
from . import transaction

# from .set_admin_by_key import set_admin_by_key


def setup(dp: Dispatcher):
    transaction.setup(dp)
    dp.register_message_handler(start_handler, CommandStart(), state="*")
    # dp.register_message_handler(set_admin_by_key, lambda msg: msg.text.lower() == config.SECRET_KEY_ADMIN, state="*")
    dp.register_message_handler(bot_help, CommandHelp(), state="*")
    dp.register_callback_query_handler(start_handler, lambda c: c.data == "main_menu", state="*")
    dp.register_callback_query_handler(user_info, lambda c: c.data == "user_info", state=UserStates.main_menu)

    dp.register_callback_query_handler(start_handler, lambda c: c.data == "back", state=UserStates.user_info)

