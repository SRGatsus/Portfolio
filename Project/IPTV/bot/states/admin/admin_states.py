from aiogram.dispatcher.filters.state import State, StatesGroup


class AdminStates(StatesGroup):
    menu = State()
    tariffs = State()
    view_tariff=State()
    create_tariff = State()
    change_tariff = State()
    change_tariff_check_input = State()
    # admins = State()
    # view_admin=State()
    # change_admin= State()
    # change_admin_check_input = State()
    # add_admin=State()
    # add_admin_check_input=State()
    users = State()
    view_user = State()
    change_user = State()
    change_user_check_input = State()
    settings = State()
    change_settings = State()
    change_settings_check_input = State()