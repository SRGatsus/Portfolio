import os

from aiogram import types
from aiogram.dispatcher import FSMContext
from ipytv import playlist
from ipytv.channel import IPTVAttr, IPTVChannel

import keyboard
from bot.states.admin import AdminStates
from loader import bot
from models import User


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
    if ct_state == "ct_new":
        info_text = (f"Создание тарифа состоит из следующих этапов\n"
                     f"1)Ввод название\n"
                     f"2)Ввод описания\n"
                     f"3)Ввод цены\n"
                     f"4)Ввод срока действия подписки(в днях)\n"
                     f"5)Добавление файла с каналами\n"
                     f"6)Добавление своих ссылок (по необходимости)\n"
                     "Введите название тарифа (можно использовтаь HTML):")
        await state.update_data({"ct_state": "ct_set_name"})
    elif ct_state == "ct_set_name":
        info_text = (f"Введите описание (можно использовтаь HTML):")
        await state.update_data({"ct_state": "ct_set_description"})
    elif ct_state == "ct_set_description":
        info_text = (f"Введите цену тарифа(минимальное значение 2.0)")
        await state.update_data({"ct_state": "ct_set_price"})
    elif ct_state == "ct_set_price":
        info_text = (f"Введите кол-во дней действия тарифа(минимальное значение 2)")
        await state.update_data({"ct_state": "ct_set_days"})
    elif ct_state == "ct_set_days":
        info_text = (
            f"Отправте файл со списком каналов(.txt,.m3u,.m3u8)\n Начало файла ОБЕЗАТЕЛЬНО должно начинаться с #EXTM3U(можете его сами добавить в начало):")
        template = open(os.getcwd() + '/data/template.txt', 'rb')
        msg = await callback_query.message.answer_document(document=template, caption=info_text,
                                                           reply_markup=keyboard.state_keyboard(is_back=True))
        await state.update_data({"last_msg": msg.message_id})
        await state.update_data({"ct_state": "ct_set_list_channel"})
        return
    elif ct_state in ["ct_set_list_channel", "ct_custom_channel"]:
        if ct_state == "ct_custom_channel" and data.get("ct_custom_chanel"):
            custom_chanel = data.get("ct_custom_chanel")
            temp = data.get("ct_list_channel") + f"\n{custom_chanel}"
            pl = playlist.loads(temp)
            await state.update_data({"ct_list_channel": str(pl)})
        info_text = (
            f"Что бы завершить создание тарифа нажмите 'Далее'\n"
            f"Что бы добавить свои каналы\потоки, то отправте сообщение по следующему формату:\n"
            f"НАЗВАНИЕ;URL;СПИСОК_АТРИБУТОВ\n"
            f"Пример:\nПервый канал HD;http://localhost/GetLivePlayList;tvg-logo=http://localhost/GetLivePlayList;group-title=Tricolor\n"
            f"Допустимы список атрибутов:\n"
        )
        for val in IPTVAttr:
            info_text += f"{val.value}\n"
        msg = await callback_query.message.answer(info_text,
                                                  reply_markup=keyboard.state_keyboard(is_back=True, is_continue=True))
        await state.update_data({"last_msg": msg.message_id, "ct_state": "ct_custom_channel"})
        return

    msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.create_tariff.set()
