from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from loader import bot, db_session
from models import User, Tariff
from bot.states.admin import AdminStates


async def view(msg: types.Message|types.CallbackQuery, state: FSMContext, user: User, tariff_id=None):
    data = await state.get_data()
    if isinstance(msg, types.Message):
        tariff_id = tariff_id or msg.text
    else:
        msg = msg.message
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                               is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    try:
        tariff_id = tariff_id or data.get("t_id")
        tariff = db_session.query(Tariff).filter(Tariff.id == tariff_id).one()
        if tariff:
            await AdminStates.view_tariff.set()
            is_user = db_session.query(User).filter(User.tariff_id == tariff.id).count()
            info_text = (f"{tariff}\n"
                         f"Дата создание тарифа: {(tariff.created_at).strftime('%d-%m-%Y в %H:%M')}\n"
                         f"Дата последнего изменения тарифа: {(tariff.updated_at).strftime('%d-%m-%Y в %H:%M')}\n"
                         f"Выберете пункт меню:")
            msg = await msg.answer(info_text, reply_markup=keyboard.admin_tariff_view(is_active=tariff.is_active,
                                                                                      view_delete=(is_user == 0)))
            await state.update_data({"last_msg": msg.message_id, "t_id": tariff.id})
        else:
            info_text = f"Тариф не найден, попробуйте еще раз"
            msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
            await state.update_data({"last_msg": msg.message_id})
    except Exception as e:
        info_text = f"Тариф не найден, попробуйте еще раз"
        msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
        await state.update_data({"last_msg": msg.message_id})
