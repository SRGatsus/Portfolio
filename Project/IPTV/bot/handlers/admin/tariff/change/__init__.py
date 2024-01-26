from aiogram import Dispatcher

from .init_input import init_input
from .check_input import check_input
from .confirmation_input import confirmation_input
from .again_input import again_input
from bot.states.admin import AdminStates




# from .set_admin_by_key import set_admin_by_key


def setup(dp: Dispatcher):

    dp.register_message_handler(check_input, content_types=["text", "document"],
                                state=AdminStates.change_tariff)
    dp.register_callback_query_handler(init_input, lambda c: str(c.data).startswith("t_change"),
                                       state=AdminStates.view_tariff)

    dp.register_callback_query_handler(confirmation_input, lambda c: c.data == "yes",
                                       state=AdminStates.change_tariff_check_input)
    dp.register_callback_query_handler(again_input, lambda c: c.data == "no",
                                       state=AdminStates.change_tariff_check_input)
