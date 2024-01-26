from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.user import UserStates
from loader import db_session
from models import User, Transaction


async def view_transaction(message: types.Message, state: FSMContext, user: User):
    text = message.text
    if text.isdigit() and int(text) >= 0 and db_session.query(Transaction).filter(Transaction.user_id == user.id,
                                                                                  Transaction.id == int(
                                                                                          text)).count() > 0:
        id_transaction = int(text)
        transaction = db_session.query(Transaction).filter(Transaction.user_id == user.id,
                                                           Transaction.id == id_transaction).one()
        info_text = f"{transaction}"
        await UserStates.view_transaction.set()
    else:
        info_text = f"Вы ввели не допустимое значение, попробуйте еще раз"
    msg = await message.answer(info_text, reply_markup=keyboard.back_keyboard())
    await state.update_data({"last_msg": msg.message_id})
