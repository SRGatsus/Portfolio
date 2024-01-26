from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from loader import bot, db_session
from models import User, Tariff, Settings
from bot.states.admin import AdminStates
from utils.misc.logging import logger

async def view(msg: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await msg.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                               is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    try:
        settings = db_session.query(Settings).all()
        settings: Settings = settings[0]
        info_text = (f"Имеются следующие настройки:\n"
                     f"Максимальное кол-во подключение на одного пользователя:{settings.max_connection}\n")
        msg = await msg.message.answer(info_text, reply_markup=keyboard.admin_settings_view())
        await state.update_data({"last_msg": msg.message_id})
    except Exception as e:
        info_text = f"Ой что то пошло не так, попробуйте позже"
        logger.error(f"Error view settings: {e}")
        msg = await msg.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
        await state.update_data({"last_msg": msg.message_id})
    await AdminStates.settings.set()
