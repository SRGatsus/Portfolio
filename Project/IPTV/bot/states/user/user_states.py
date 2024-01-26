from aiogram.dispatcher.filters.state import State, StatesGroup


class UserStates(StatesGroup):
    main_menu=State()
    admin_menu=State()
    admin_tariff = State()
    user_info = State()
    list_transaction = State()
    view_transaction = State()

