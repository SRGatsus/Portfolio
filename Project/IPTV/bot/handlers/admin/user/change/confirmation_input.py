import datetime

from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from loader import bot, db_session
from models import User, Tariff, Transaction
from ..every import every
from ..view import view


async def confirmation_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("cu_state")
    info_text = ""
    client: User = db_session.query(User).filter(User.id == data.get("u_id")).one()
    if ct_state == "cu_add_tariff":
        tariff = db_session.query(Tariff).filter(Tariff.id == data.get("сa_tariff")).one()
        client.tariff_status = 1
        client.tariff_id = tariff.id
        client.tariff_start = datetime.datetime.now()
        client.tariff_end = datetime.datetime.now() + datetime.timedelta(days=tariff.days)
        label = f"admin_add_tariff:{str(datetime.datetime.now().microsecond)}:{tariff.id}:0"
        transaction = Transaction(created_at=datetime.datetime.now(), updated_at=datetime.datetime.now(), label=label,
                                  user_id=client.id, is_payment=0, is_admin_check=1,is_close=1)
        db_session.add_model(transaction)
        await bot.send_message(chat_id=client.telegram_id, text=f"Вам добавили подписку")
    elif ct_state == "cu_change_tariff":
        label = f"admin_change_tariff:{str(datetime.datetime.now().microsecond)}:{client.tariff_id}:{data.get('сa_tariff')}:0"
        transaction = Transaction(created_at=datetime.datetime.now(), updated_at=datetime.datetime.now(), label=label,
                                  user_id=client.id, is_payment=0, is_admin_check=1,is_close=1)
        client.tariff_id = data.get("сa_tariff")
        db_session.add_model(transaction)
        await bot.send_message(chat_id=client.telegram_id, text=f"Вам изменили подписку")
    elif ct_state == "cu_blocked":
        client.is_admin = False
        client.is_blocked = True
        await bot.send_message(chat_id=client.telegram_id, parse_mode=types.ParseMode.HTML,
                               text=f"К сожелению вас заблокировали по причине:\n{data.get('сa_blocked')}")
        update_data = datetime.datetime.now()
        client.updated_at = update_data
        db_session.commit_session()
        await callback_query.message.answer("Успешно обновлено")
        await every(callback_query, state, user)
        return
    elif ct_state == "cu_list_transaction":
        transaction = db_session.query(Transaction).filter(
            Transaction.user_id == data.get("u_id"), Transaction.id == data.get("сa_transaction")).one()
        info_text = f"{transaction}"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
        await state.update_data({"last_msg": msg.message_id})
        return
    update_data = datetime.datetime.now()
    client.updated_at = update_data
    db_session.commit_session()
    await callback_query.message.answer("Успешно обновлено")
    await view(msg=callback_query.message, state=state, user=user, admin_id=client.id)
