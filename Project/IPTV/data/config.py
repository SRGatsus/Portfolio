from pathlib import Path
from decouple import config

DIR = Path(__file__).absolute().parent
TELEGRAM_BOT_TOKEN = config("TELEGRAM_BOT_TOKEN")
ADMINS = [int(_) for _ in config("ADMINS", default="").split(",")]
MAIN_ADMIN = config("MAIN_ADMINS", default="")
RATE_LIMIT = config("RATE_LIMIT", default=0.5, cast=float)
SECRET_KEY = config("SECRET_KEY",default="SECRET_KEY")
SECRET_KEY_ADMIN = config("SECRET_KEY_ADMIN",default="SECRET_KEY_ADMIN")
SITE_URL_BASE = config("SITE_URL_BASE")
SITE_URL_LIVE = config("SITE_URL_LIVE")
YOOMONEY_ACCOUNT_NUMBER = config("YOOMONEY_ACCOUNT_NUMBER")
YOOMONEY_ACCOUNT_TOKEN = config("YOOMONEY_ACCOUNT_TOKEN")
YOOMONEY_CLIENT_ID = config("YOOMONEY_CLIENT_ID")
YOOMONEY_URL_BASE = config("YOOMONEY_URL_BASE")
YOOMONEY_URL_REDIRECT = config("YOOMONEY_URL_REDIRECT")

REDIS_HOST = config("REDIS_HOST", default=None)
REDIS_PORT = config("REDIS_PORT", default=6379, cast=int)
REDIS_DB = config("REDIS_DB", default=2)

if REDIS_PORT:
    REDIS_PORT = int(REDIS_PORT)
if REDIS_DB:
    REDIS_DB = int(REDIS_DB)


LOGS_BASE_PATH = config(
    "LOGS_BASE_PATH", default=str(Path(__file__).parent.parent / "logs")
)
