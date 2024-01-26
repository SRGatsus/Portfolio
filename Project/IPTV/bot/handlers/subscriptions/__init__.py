from aiogram import Dispatcher
from .view_sub import view_sub

# from .set_admin_by_key import set_admin_by_key


def setup(dp: Dispatcher):
    dp.register_callback_query_handler(view_sub, lambda c: c.data == "user_sub", state="*")
    # dp.register_callback_query_handler(user_info, lambda c: c.data == "user_info", state=UserStates.main_menu)
    #
    # dp.register_callback_query_handler(start_handler, lambda c: c.data == "back", state=UserStates.user_info)
