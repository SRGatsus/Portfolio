from aiogram import types
from aiogram.dispatcher import FSMContext
from bot.states.admin import AdminStates
import keyboard
from loader import bot, db_session
from models import Tariff, User
from utils.misc.logging import logger

async def every(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    if isinstance(callback_query, types.CallbackQuery) and callback_query.data.startswith("subscriptions"):
        _, page = callback_query.data.split(";")
        page = int(page)
    else:
        page = 1
    page_size = 50
    offset_data = page_size * (page - 1)
    try:
        tariffs = db_session.query(Tariff).offset(offset=offset_data).limit(limit=page_size).all()
        info_text = (f"Введите номер тарифа для подробной информации о нем\n"
                     f"Обозначения:\n"
                     f"🟢 - тариф активен\n"
                     f"🟡 - тариф неактивен, но есть пользователь пользующийся им\n"
                     f"🔴 - тариф неактивен\n"
                     f"Список тарифов:\n")
        if len(tariffs) == 0:
            info_text+=f"Пусто\n"
        else:
            for tariff in tariffs:
                if tariff.is_active:
                    info_text += f"🟢 {tariff.id}) {tariff.title}\n"
                else:
                    is_user = db_session.query(User).filter(User.tariff_id == tariff.id).count()
                    if is_user>0:
                        info_text += f"🟡 {tariff.id}) {tariff.title}\n"
                    else:
                        info_text += f"🔴 {tariff.id}) {tariff.title}\n"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.pagination_keyboard(data_count=len(tariffs),
                                                                                            data_max=page_size,
                                                                                            prefix="subscriptions",
                                                                                            page=page
                                                                                            ))
    except Exception as e:
        logger.warning(f"Search tariffs pagination({page},{page_size}) - error:{e}")
        msg = await callback_query.message.answer("При поиске произошла ошибка, попробуйте позже",
                                                  reply_markup=keyboard.back_keyboard())
    await state.update_data({"last_msg": msg.message_id,"t_id":None})
    await AdminStates.tariffs.set()
