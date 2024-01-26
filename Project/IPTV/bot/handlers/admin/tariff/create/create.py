import datetime

from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from loader import bot, db_session
from models import User, Tariff
from bot.states.admin import AdminStates


async def create(msg: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await msg.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                               is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    try:
        info_text = "Создается тариф, подождите пожалуйста"
        msg = await msg.message.answer(info_text)
        create_data = update_data = datetime.datetime.now()
        tariff = Tariff(price=data.get("ct_price"), title=data.get("ct_name"), description=data.get("ct_description"),
                        days=data.get("ct_days"), is_active=1, list_channel=data.get("ct_list_channel"),
                        created_at=create_data,
                        updated_at=update_data)

        db_session.add_model(tariff)
        info_text = f"Был создан тариф:\n{tariff}"
        msg = await msg.answer(info_text, reply_markup=keyboard.admin_tariff_view(is_active=tariff.is_active))
        await state.reset_data()
        await state.update_data({"last_msg": msg.message_id})
        await AdminStates.view_tariff.set()
        db_session.commit_session()
    except Exception as e:
        print(e)


