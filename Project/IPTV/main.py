from aiogram import Dispatcher, executor
from aiohttp import web
from loguru import logger

from loader import dp, bot


async def on_startup(dp: Dispatcher):
    logger.info("Bot startup")

    from utils.misc import logging
    from bot import handlers
    from bot import middlewares

    logging.setup()
    middlewares.setup(dp)
    handlers.user.setup(dp)
    handlers.subscriptions.setup(dp)
    handlers.admin.setup(dp)


async def on_shutdown(dp: Dispatcher):
    logger.warning("Shutting down..")
    await dp.storage.close()
    await dp.storage.wait_closed()


async def on_shutdown_web_application(app: web.Application):
    logger.warning("Shutting down web application..")
    app_bot = app['bot']
    await on_shutdown(dp)
    await app_bot.close()


def main():
    executor.start_polling(
        dp, on_startup=on_startup, on_shutdown=on_shutdown, skip_updates=True
    )


if __name__ == "__main__":
    try:
        main()
    except (KeyboardInterrupt, SystemExit):
        logger.error("Bot stopped!")
