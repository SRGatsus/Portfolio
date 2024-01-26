from aiogram.types import InlineKeyboardMarkup, InlineKeyboardButton


def start_keyboard(is_admin=False):
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    user_info = InlineKeyboardButton("Обо мне", callback_data="user_info")
    user_sub = InlineKeyboardButton("Подписка", callback_data="user_sub")
    admin_panel = InlineKeyboardButton("Админ панель", callback_data="admin_panel")
    keyboard.add(user_info, user_sub)
    if is_admin:
        keyboard.add(admin_panel)
    return keyboard


def pagination_keyboard(data_count, data_max, prefix, page=1):
    keyboard = InlineKeyboardMarkup(resize=True)
    row = []
    if page > 1:
        left_btn = InlineKeyboardButton(
            "⬅️", callback_data=f"{prefix};{page - 1}"
        )
        row.append(left_btn)

    if data_count == data_max:
        right_btn = InlineKeyboardButton(
            "➡️", callback_data=f"{prefix};{page + 1}"
        )
        row.append(right_btn)
    keyboard.row(*row)
    keyboard.add(InlineKeyboardButton("Назад", callback_data="back"))
    return keyboard


def state_keyboard(is_continue=False, is_main_menu=False, is_back=True, is_repeat=False, is_yes=False):
    keyboard = InlineKeyboardMarkup(resize=True)
    continue_btn = InlineKeyboardButton("Далее", callback_data="continue")
    yes_btn = InlineKeyboardButton("Да", callback_data="yes")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    no_btn = InlineKeyboardButton("Нет", callback_data="no")
    main_menu_btn = InlineKeyboardButton("Главное меню", callback_data="main_menu")
    if is_yes:
        keyboard.add(yes_btn, no_btn)
    if is_continue:
        keyboard.add(back_btn, continue_btn)
    else:
        if is_back:
            keyboard.add(back_btn)

    if is_main_menu:
        keyboard.add(main_menu_btn)
    return keyboard


def admin_settings_view():
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    max_connection_btn = InlineKeyboardButton("Макс. подключений", callback_data="s_change;max_connection")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    keyboard.add(max_connection_btn)
    keyboard.add(back_btn)
    return keyboard


def admin_tariff_view(is_active=False, view_delete=False):
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    change_tariff_name = InlineKeyboardButton("Изменить название", callback_data="t_change;name")
    change_tariff_description = InlineKeyboardButton("Изменить описание", callback_data="t_change;description")
    change_tariff_price = InlineKeyboardButton("Изменить цену", callback_data="t_change;price")
    change_tariff_days = InlineKeyboardButton("Изменить кол-во дней", callback_data="t_change;days")
    change_tariff_list_channel = InlineKeyboardButton("Заменить список каналов", callback_data="t_change;list_channel")
    change_tariff_custom_channel = InlineKeyboardButton("Добавить канал", callback_data="t_change;custom_channel")
    change_tariff_delete_channel = InlineKeyboardButton("Удалить канал", callback_data="t_change;delete_channel")
    change_tariff_all_list_channel = InlineKeyboardButton("Список каналов", callback_data="t_change;all_list_channel")
    change_tariff_file_channel = InlineKeyboardButton("Файл", callback_data="t_change;file_channel")
    change_tariff_link_channel = InlineKeyboardButton("Тестовая ссылка", callback_data="t_change;link_channel")
    tariff_deactivate = InlineKeyboardButton("Деактивировать", callback_data="t_change;deactivate")
    tariff_activate = InlineKeyboardButton("Активировать", callback_data="t_change;activate")
    tariff_delete = InlineKeyboardButton("Удалить", callback_data="t_change;delete")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    keyboard.add(change_tariff_name, change_tariff_description, change_tariff_price, change_tariff_days,
                 change_tariff_all_list_channel, change_tariff_list_channel, change_tariff_custom_channel,
                 change_tariff_delete_channel)
    keyboard.add(change_tariff_file_channel, change_tariff_link_channel)
    if is_active:
        keyboard.add(tariff_deactivate)
    else:
        keyboard.add(tariff_activate)
    if view_delete:
        keyboard.add(tariff_delete)
    keyboard.add(back_btn)
    return keyboard


def admin_keyboard():
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    tariffs = InlineKeyboardButton("Тарифы", callback_data="subscriptions;1")
    users = InlineKeyboardButton("Пользователи", callback_data="users;1")
    settings = InlineKeyboardButton("Настройки", callback_data="settings")
    main_menu_btn = InlineKeyboardButton("Главное меню", callback_data="main_menu")
    keyboard.add(tariffs, users)
    keyboard.add(settings)
    keyboard.add(main_menu_btn)
    return keyboard


def admin_admin_view(is_tariff=False, is_transaction=False, is_connection=False, is_on_tariff=False):
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    add_tariff = InlineKeyboardButton("Добавить тариф", callback_data="a_change;add_tariff")
    off_tariff = InlineKeyboardButton("Отключить тариф", callback_data="a_change;off_tariff")
    on_tariff = InlineKeyboardButton("Включить тариф", callback_data="a_change;on_tariff")
    change_tariff = InlineKeyboardButton("Изменить тариф", callback_data="a_change;change_tariff")
    prolong_tariff = InlineKeyboardButton("Продлить тариф", callback_data="a_change;prolong_tariff")
    delete_tariff = InlineKeyboardButton("Удалить тариф", callback_data="a_change;delete_tariff")
    list_transaction = InlineKeyboardButton("Список транзакций", callback_data="a_change;list_transaction")
    list_connection = InlineKeyboardButton("Список подключений", callback_data="a_change;list_connection")
    deactivate = InlineKeyboardButton("Отключить Админ Панель", callback_data="a_change;deactivate")
    blocked = InlineKeyboardButton("Заблокировать", callback_data="a_change;blocked")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    if is_tariff:
        if is_on_tariff:
            keyboard.add(change_tariff, off_tariff)
        else:
            keyboard.add(change_tariff, on_tariff)
        keyboard.add(prolong_tariff, delete_tariff)
    else:
        keyboard.add(add_tariff)
    if is_connection and is_transaction:
        keyboard.add(list_connection, list_connection)
    elif is_transaction:
        keyboard.add(list_transaction)
    elif is_connection:
        keyboard.add(list_connection)
    keyboard.add(deactivate)
    keyboard.add(blocked)
    keyboard.add(back_btn)
    return keyboard


def admin_user_view(is_tariff=False, is_transaction=False, is_connection=False, is_on_tariff=False, is_blocked=False,
                    is_admin=False):
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    add_tariff = InlineKeyboardButton("Добавить тариф", callback_data="u_change;add_tariff;1")
    off_tariff = InlineKeyboardButton("Отключить тариф", callback_data="u_change;off_tariff")
    on_tariff = InlineKeyboardButton("Включить тариф", callback_data="u_change;on_tariff")
    change_tariff = InlineKeyboardButton("Изменить тариф", callback_data="u_change;change_tariff;1")
    prolong_tariff = InlineKeyboardButton("Продлить тариф", callback_data="u_change;prolong_tariff")
    delete_tariff = InlineKeyboardButton("Удалить тариф", callback_data="u_change;delete_tariff")
    list_transaction = InlineKeyboardButton("Список транзакций", callback_data="u_change;list_transaction;1")
    list_connection = InlineKeyboardButton("Список подключений", callback_data="u_change;list_connection;1")
    deactivate = InlineKeyboardButton("Отключить Админ Панель", callback_data="u_change;deactivate")
    activate = InlineKeyboardButton("Включить Админ Панель", callback_data="u_change;activate")
    blocked = InlineKeyboardButton("Заблокировать", callback_data="u_change;blocked")
    unblocked = InlineKeyboardButton("Разблокировать", callback_data="u_change;unblocked")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    if is_tariff:
        if is_on_tariff:
            keyboard.add(change_tariff, off_tariff)
        else:
            keyboard.add(change_tariff, on_tariff)
        keyboard.add(prolong_tariff, delete_tariff)
    else:
        keyboard.add(add_tariff)
    if is_connection and is_transaction:
        keyboard.add(list_connection, list_connection)
    elif is_transaction:
        keyboard.add(list_transaction)
    elif is_connection:
        keyboard.add(list_connection)
    if is_admin:
        keyboard.add(deactivate)
    else:
        keyboard.add(activate)
    if is_blocked:
        keyboard.add(unblocked)
    else:
        keyboard.add(blocked)
    keyboard.add(back_btn)
    return keyboard


def user_view(is_transaction=False, is_connection=False, is_tariff=False):
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    prolong_tariff = InlineKeyboardButton("Продлить тариф", callback_data="prolong_tariff")
    list_transaction = InlineKeyboardButton("Список транзакций", callback_data="list_transaction;1")
    list_connection = InlineKeyboardButton("Список подключений", callback_data="list_connection;1")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    if is_tariff:
        keyboard.add(prolong_tariff)
    if is_connection and is_transaction:
        keyboard.add(list_connection, list_connection)
    elif is_transaction:
        keyboard.add(list_transaction)
    elif is_connection:
        keyboard.add(list_connection)
    keyboard.add(back_btn)
    return keyboard


def admin_admins():
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    add_tariff = InlineKeyboardButton("Добавить админа", callback_data="a_add;add")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    keyboard.add(add_tariff)
    keyboard.add(back_btn)
    return keyboard


def admin_tariffs():
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    add_tariff = InlineKeyboardButton("Создать тариф", callback_data="create")
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    keyboard.add(add_tariff)
    keyboard.add(back_btn)
    return keyboard


def view_sub_keyboard(view_check=False, is_active_tariff=False):
    keyboard = InlineKeyboardMarkup(resize=True, row_width=2)
    check_transaction_btn = InlineKeyboardButton("Проверить оплату", callback_data="check_paid")
    get_url_btn = InlineKeyboardButton("Получить ссылку", callback_data="get_url")
    get_file_btn = InlineKeyboardButton("Получить файл", callback_data="get_file")
    get_all_tariff = InlineKeyboardButton("Другие тарифы", callback_data="subscriptions;1")
    prolong_tariff = InlineKeyboardButton("Продлить тариф", callback_data="prolong_tariff")
    if not is_active_tariff:
        get_all_tariff = InlineKeyboardButton("Тарифы", callback_data="subscriptions;1")
    if is_active_tariff:
        keyboard.add(get_url_btn, get_file_btn)
        keyboard.add(prolong_tariff)
    keyboard.add(get_all_tariff)
    if view_check:
        keyboard.add(check_transaction_btn)
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    keyboard.add(back_btn)
    return keyboard


def back_keyboard():
    keyboard = InlineKeyboardMarkup(resize=True)
    back_btn = InlineKeyboardButton("Назад", callback_data="back")
    keyboard.add(back_btn)
    return keyboard
