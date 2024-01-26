from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.admin import AdminStates
from models import User


async def again_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    flag = True
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("s_state")
    info_text = ""
    if ct_state == "max_connection":
        info_text += ("Введите новое значений:")
        await state.update_data({"s_state": "max_connection"})

    msg = await callback_query.message.answer(info_text, parse_mode=types.ParseMode.HTML,
                                              reply_markup=keyboard.state_keyboard(is_back=True))
    await state.update_data({"last_msg": msg.message_id})
    if flag:
        await AdminStates.change_settings_check_input.set()
