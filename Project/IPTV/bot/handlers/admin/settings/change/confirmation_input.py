import datetime

from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from loader import db_session
from models import User, Settings
from ..view import view


async def confirmation_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("s_state")
    info_text = ""
    settings = db_session.query(Settings).all()
    settings: Settings = settings[0]
    if ct_state == "max_connection":
        settings.max_connection = data.get("s_max_connection")
    update_data = datetime.datetime.now()
    settings.updated_at = update_data
    db_session.commit_session()
    await callback_query.message.answer("Успешно обновлено")
    await view(msg=callback_query, state=state, user=user)
