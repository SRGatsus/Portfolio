from aiogram import Dispatcher


def setup(dp: Dispatcher):
    from .logging import LoggingMiddleware
    from .user import UsersMiddleware
    dp.middleware.setup(LoggingMiddleware())
    dp.middleware.setup(UsersMiddleware())
