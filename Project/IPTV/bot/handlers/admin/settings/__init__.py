from aiogram import Dispatcher

from bot.states.admin import AdminStates
from .view import view
from . import change


def setup(dp: Dispatcher):
    change.setup(dp)
    dp.register_callback_query_handler(view, lambda c: str(c.data).startswith("settings"), state=AdminStates.menu)
    dp.register_callback_query_handler(view, lambda c: c.data == "back",
                                       state=[AdminStates.change_settings,
                                              AdminStates.change_settings_check_input])
