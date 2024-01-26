from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.user import UserStates
from loader import db_session
from models import User, Transaction
from models.transaction import typesTransaction


async def list_transactions(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    info_text = "Введите номер транзакции для подробной информации о нем\n"
    try:
        if isinstance(callback_query, types.CallbackQuery) and callback_query.data.startswith("list_transaction"):
            _, page = callback_query.data.split(";")
            page = int(page)
        else:
            page = 0
        page_size = 50
        offset_data = page_size * (page - 1)
        transactions = db_session.query(Transaction).filter(Transaction.user_id == user.id).order_by(
            Transaction.id.desc()).offset(offset=offset_data).limit(limit=page_size).all()
        for transaction in transactions:
            info_text += f"{transaction.id}) {typesTransaction.get(transaction.label.split(':')[0], 'Не известный тип')}\n"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.pagination_keyboard(
            data_count=len(transactions),
            data_max=page_size,
            prefix="list_transaction",
            page=page
            ))
    except Exception as e:
        info_text += "Транзакции не найдены"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.back_keyboard())
    await state.update_data({"last_msg": msg.message_id})
    await UserStates.list_transaction.set()
