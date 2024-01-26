from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.user import UserStates
from loader import db_session
from models import User, Tariff, Transaction, Connection


async def user_info(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    info_text = str(user) + "\n"
    if user.tariff_id > -1:
        tariff = db_session.query(Tariff).filter(Tariff.id == user.tariff_id).one()
        status = "неактивен"
        if user.tariff_status == 1:
            status = "активен"
        elif user.tariff_status == 2:
            status = "отключен"
        info_text += (f"Имеется тариф:\n{tariff}"
                      f"Статус: {status}\n"
                      f"Дата подключение: {(user.tariff_start).strftime('%d-%m-%Y в %H:%M')}\n"
                      f"Дата окончания: {(user.tariff_end).strftime('%d-%m-%Y в %H:%M')}\n")
    is_transaction = (db_session.query(Transaction).filter(Transaction.user_id == user.id).count()) > 0
    is_connection = (db_session.query(Connection).filter(Connection.user_id == user.id).count()) > 0
    msg = await callback_query.message.answer(info_text, reply_markup=keyboard.user_view(is_transaction=is_transaction,
                                                                                         is_connection=is_connection,
                                                                                         is_tariff=user.tariff_id > -1))
    await state.update_data({"last_msg": msg.message_id})
    await UserStates.user_info.set()
