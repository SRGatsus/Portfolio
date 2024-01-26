from aiogram import Dispatcher

from . import settings
from . import tariff
from . import user
from .admin_panel import admin_panel
from ...states.admin import AdminStates
from ...states.user import UserStates


# from .set_admin_by_key import set_admin_by_key


def setup(dp: Dispatcher):
    tariff.setup(dp)
    user.setup(dp)
    settings.setup(dp)

    dp.register_callback_query_handler(admin_panel, lambda c: c.data == "admin_panel", state=UserStates.main_menu)

    dp.register_callback_query_handler(admin_panel, lambda c: c.data == "back",
                                       state=[AdminStates.menu, AdminStates.tariffs, AdminStates.users,
                                              AdminStates.settings])
