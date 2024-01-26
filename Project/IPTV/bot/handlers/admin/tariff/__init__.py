from aiogram import Dispatcher

from bot.states.admin import AdminStates
from . import change
from . import create
from .every import every
from .view import view


# from .set_admin_by_key import set_admin_by_key


def setup(dp: Dispatcher):
    create.setup(dp)
    change.setup(dp)
    dp.register_callback_query_handler(every, lambda c: str(c.data).startswith("subscriptions") , state=AdminStates.menu)
    dp.register_message_handler(view, content_types=["text"],
                                state=AdminStates.tariffs)
    dp.register_callback_query_handler(every, lambda c: c.data == "back",
                                       state=[AdminStates.create_tariff,  AdminStates.view_tariff])
    dp.register_callback_query_handler(view, lambda c: c.data == "back",
                                       state=[AdminStates.change_tariff,
                                              AdminStates.change_tariff_check_input,])
