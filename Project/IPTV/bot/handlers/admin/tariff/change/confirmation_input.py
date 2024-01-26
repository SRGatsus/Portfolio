import datetime

from aiogram import types
from aiogram.dispatcher import FSMContext
from ipytv import playlist

import keyboard
from bot.handlers.admin.tariff.view import view
from loader import bot, db_session
from models import User, Tariff


async def confirmation_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("ct_state", "ct_new")
    info_text = ""
    tariff = db_session.query(Tariff).filter(Tariff.id == data.get("t_id")).one()
    if ct_state == "ct_set_name":
        tariff.title = data.get("ct_name")
    elif ct_state == "ct_set_description":
        tariff.description = data.get("ct_description")
    elif ct_state == "ct_set_price":
        tariff.price = data.get("ct_price")
    elif ct_state == "ct_set_days":
        tariff.days = data.get("ct_days")
    elif ct_state == "ct_set_list_channel":
        tariff.list_channel = data.get("ct_list_channel")
    elif ct_state == "ct_custom_channel":
        tariff.list_channel += f"\n{data.get('ct_custom_chanel')}"
    elif ct_state == "ct_delete_channel":
        await callback_query.message.answer("Подождите чуть чуть, удаляем канал")
        pl = playlist.loads(tariff.list_channel)
        pl.get_channels().pop(data.get("ct_delete_channel"))
        tariff.list_channel = str(pl)
    update_data = datetime.datetime.now()
    tariff.updated_at = update_data
    db_session.commit_session()
    await callback_query.message.answer("Тариф обновлен")
    await view(msg=callback_query.message, state=state, user=user, tariff_id=tariff.id)
