from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.admin import AdminStates
from loader import bot, db_session
from models import User, Tariff, Transaction
from models.transaction import typesTransaction


def isNumber(s):
    try:
        float(s)
        return True
    except ValueError:
        return False


async def check_input(msg: types.Message, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                               is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("cu_state")
    text = msg.text
    info_text = ""
    reply_markup = keyboard.state_keyboard(is_back=True)
    if ct_state == "cu_add_tariff":
        if text.isdigit() and int(text) >= 0 and db_session.query(Tariff).filter(Tariff.id == int(text)).count() > 0:
            id_tariff = int(text)
            tariff = db_session.query(Tariff).filter(Tariff.id == id_tariff).one()
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            info_text = (f"Вы хотите добавить тариф:\n{tariff}\n"
                         f"Пользователю:\n{admin}\n"
                         f"Все верно?")
            await state.update_data({"сa_tariff": id_tariff})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели не допустимое значение, попробуйте еще раз"
    elif ct_state == "cu_change_tariff":
        if text.isdigit() and int(text) >= 0 and db_session.query(Tariff).filter(Tariff.id == int(text)).count() > 0:
            id_tariff = int(text)
            admin: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
            tariff = db_session.query(Tariff).filter(Tariff.id == id_tariff).one()
            tariff_old = db_session.query(Tariff).filter(Tariff.id == admin.tariff_id).one()
            info_text = (f"Вы хотите поменять тариф с:\n{tariff_old}\n\n"
                         f"На:\n{tariff}\n\n"
                         f"Пользователю:\n{admin}\n"
                         f"Все верно?")
            await state.update_data({"сa_tariff": id_tariff})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели не допустимое значение, попробуйте еще раз"
    elif ct_state == "cu_blocked":
        if 30 <= len(text) <= 1200:
            info_text = f"Вы ввели причну блокировки:\n{text}\nВсе верно?"
            await state.update_data({"сa_blocked": text})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели текст меньше 30 символов или больше 1200.\nТекущая длинна: {len(text)}\nВведите заново текст"
    elif ct_state == "cu_list_transaction":
        if text.isdigit() and int(text) >= 0 and db_session.query(Transaction).filter(
                Transaction.user_id == data.get("u_id"), Transaction.id == int(text)).count() > 0:
            id_transaction = int(text)
            transaction = db_session.query(Transaction).filter(
                Transaction.user_id == data.get("u_id"), Transaction.id == id_transaction).one()
            info_text = (f"Вы хотите посмотреть транзакцию:\n"
                         f"{transaction.id} {typesTransaction.get(transaction.label.split(':')[0], 'Не известный тип')}\n"
                         f"Все верно?")
            await state.update_data({"сa_transaction": id_transaction})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели не допустимое значение, попробуйте еще раз"
    else:
        info_text = f"Все верно?"
        reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
    arrayMsg = []
    tmp = ""
    for value in info_text.split("\n\n"):
        if len(tmp + value + "\n\n") <= 4096:
            tmp += value + "\n\n"
        else:
            arrayMsg.append(tmp)
            tmp = value + "\n\n"
    arrayMsg.append(tmp)
    for x in range(len(arrayMsg)):
        if x + 1 == len(arrayMsg):
            msg = await msg.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML, reply_markup=reply_markup)
        else:
            await msg.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML, )
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.change_user_check_input.set()
