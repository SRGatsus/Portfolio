from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states import SubStates
from loader import db_session
from models import User, Tariff, Transaction
from models.transaction import typesTransaction


async def view_sub(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    info_text = ""
    view_check = False
    is_active_tariff = False
    if user.tariff_id and user.tariff_id > 0:
        tariff = db_session.query(Tariff).filter(Tariff.id == user.tariff_id).one()
        status = "неактивен"
        if user.tariff_status == 1:
            status = "активен"
            is_active_tariff = True
        elif user.tariff_status == 2:
            status = "отключен"
        info_text += (f"Имеется тариф:\n{tariff}"
                      f"Статус: {status}\n"
                      f"Дата подключение: {(user.tariff_start).strftime('%d-%m-%Y в %H:%M')}\n"
                      f"Дата окончания: {(user.tariff_end).strftime('%d-%m-%Y в %H:%M')}\n")
    else:
        info_text = f"У вас нет тарифа\n"
    active_transactions = db_session.query(Transaction).filter(Transaction.user_id == user.id,
                                                               Transaction.is_close == False).all()
    if len(active_transactions) > 0:
        info_text += "У вас имеется открытые транзакции:\n"
        for transaction in active_transactions:
            info_text += f"{transaction.id}) {typesTransaction.get(transaction.label.split(':')[0], 'Не известный тип')}\n"
            view_check = True
    msg = await callback_query.message.answer(info_text, reply_markup=keyboard.view_sub_keyboard(view_check=view_check,
                                                                                                 is_active_tariff=is_active_tariff))
    await state.update_data({"last_msg": msg.message_id})
    await SubStates.view_sub.set()
