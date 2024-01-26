from aiogram import Dispatcher


def setup(dp: Dispatcher):
    from .log_any_error import log_any_error

    dp.register_errors_handler(log_any_error)
