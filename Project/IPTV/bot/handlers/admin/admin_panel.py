from aiogram import types
from aiogram.dispatcher import FSMContext
from bot.states.admin import AdminStates
import keyboard
from loader import bot
from models import User


async def admin_panel(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    await state.reset_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    info_text = f"Добро пожаловать в админ панель\nВыберете пункт меню:"
    msg = await callback_query.message.answer(info_text, reply_markup=keyboard.admin_keyboard())
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.menu.set()
