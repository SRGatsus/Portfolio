from aiogram import Dispatcher

from bot.states.admin import AdminStates
from .every import every
from .view import view
from . import change


def setup(dp: Dispatcher):
    change.setup(dp)
    dp.register_callback_query_handler(every, lambda c: str(c.data).startswith("users"), state=[AdminStates.menu,AdminStates.users])
    dp.register_message_handler(view, content_types=["text"],
                                state=AdminStates.users)
    dp.register_callback_query_handler(every, lambda c: c.data == "back",
                                       state=[AdminStates.view_user,AdminStates.change_user_check_input,AdminStates.change_user])
