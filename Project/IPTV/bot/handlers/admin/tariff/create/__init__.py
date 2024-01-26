from aiogram import Dispatcher

from .confirmation_input import confirmation_input
from .again_input import again_input
from .check_input import check_input
from .create import create
from bot.states.admin import AdminStates


def setup(dp: Dispatcher):
    dp.register_callback_query_handler(confirmation_input, lambda c: c.data == "create",
                                       state=AdminStates.tariffs)
    dp.register_message_handler(check_input, content_types=["text", "document"],
                                state=AdminStates.create_tariff)
    dp.register_callback_query_handler(create, lambda c: c.data == "continue",
                                       state=AdminStates.create_tariff)
    dp.register_callback_query_handler(confirmation_input, lambda c: c.data == "yes",
                                       state=AdminStates.create_tariff)
    dp.register_callback_query_handler(again_input, lambda c: c.data == "no",
                                       state=AdminStates.create_tariff)
