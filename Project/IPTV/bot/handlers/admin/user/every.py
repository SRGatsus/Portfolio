from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.admin import AdminStates
from loader import db_session
from models import User
from utils.misc.logging import logger

async def every(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    if isinstance(callback_query, types.CallbackQuery) and callback_query.data.startswith("users"):
        _, page = callback_query.data.split(";")
        page = int(page)
    else:
        page = 1
    page_size = 50
    offset_data = page_size * (page - 1)
    try:
        users = db_session.query(User).order_by(User.is_admin.desc()).offset(offset=offset_data).limit(limit=page_size).all()
        info_text = (f"Введите id пользователя для подробной информации о нем\n"
                     f"Обозначения:\n"
                     f"🟢 - пользователь\n"
                     f"🟡 - администратор\n"
                     f"🔴 - пользователь заблокирован\n"
                     f"Список пользователей:\n")
        if len(users) == 0:
            info_text += f"Пусто\n"
        else:
            for user_value in users:
                if user_value.is_blocked:
                    info_text += f"🔴 {user_value.id}) {user_value.username} - tg_id:{user_value.telegram_id}"
                elif user_value.is_admin:
                    info_text += f"🟡 {user_value.id}) {user_value.username} - tg_id:{user_value.telegram_id}"
                else:
                    info_text += f"🟢{user_value.id}) {user_value.username} - tg_id:{user_value.telegram_id}"
                if user_value.id == user.id:
                    info_text += f" (вы)\n"
                else:
                    info_text += f"\n"
        msg = await callback_query.message.answer(info_text,
                                                  reply_markup=keyboard.pagination_keyboard(data_count=len(users),
                                                                                            data_max=page_size,
                                                                                            prefix="users",
                                                                                            page=page
                                                                                            ))
    except Exception as e:
        logger.warning(f"Search user pagination({page},{page_size}) - error:{e}")
        msg = await callback_query.message.answer("При поиске произошла ошибка, попробуйте позже",
                                                  reply_markup=keyboard.back_keyboard())
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.users.set()
