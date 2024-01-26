import os

from aiogram import types
from aiogram.dispatcher import FSMContext
from ipytv import playlist
from ipytv.channel import IPTVAttr

import keyboard
from bot.states.admin import AdminStates
from loader import bot, db_session
from models import User, Tariff


async def again_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("ct_state", "ct_new")
    info_text = ""
    if ct_state == "ct_set_name":
        info_text = ("Введите название тарифа (можно использовтаь HTML):")
        await state.update_data({"ct_state": "ct_set_name"})
    elif ct_state == "ct_set_description":
        info_text = (f"Введите описание (можно использовтаь HTML):")
        await state.update_data({"ct_state": "ct_set_description"})
    elif ct_state == "ct_set_price":
        info_text = (f"Введите цену тарифа(минимальное значение 2.0)")
        await state.update_data({"ct_state": "ct_set_price"})
    elif ct_state == "ct_set_days":
        info_text = (f"Введите кол-во дней действия тарифа(минимальное значение 2)")
        await state.update_data({"ct_state": "ct_set_days"})
    elif ct_state == "ct_set_list_channel":
        info_text = (
            f"Отправте файл со списком каналов(.txt,.m3u,.m3u8)\n Начало файла ОБЕЗАТЕЛЬНО должно начинаться с #EXTM3U(можете его сами добавить в начало):")
        template = open(os.getcwd() +'/data/template.txt', 'rb')
        msg = await callback_query.message.answer_document(document=template, caption=info_text,
                                                           reply_markup=keyboard.state_keyboard(is_back=True))
        await state.update_data({"last_msg": msg.message_id,"ct_state": "ct_set_custom_channel"})
        await AdminStates.change_tariff.set()
        return
    elif ct_state == "ct_custom_channel":
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
        await AdminStates.change_tariff.set()
        return
    elif ct_state == "ct_delete_channel":
        try:
            await callback_query.message.answer("Подождите чуть чуть, ищем каналы\nАккуратно может быть спам сообщений")
            tariff = db_session.query(Tariff).filter(Tariff.id == data.get("t_id")).one()
            pl = playlist.loads(tariff.list_channel)
            reply_markup = keyboard.state_keyboard(is_back=True)
            index = 0
            for value in pl.get_channels():
                info_text += (f"{index}){value.name}\n")
                index+=1
            info_text += "Введите номер канала который хотите удалить\n"
            arrayMsg = []
            tmp = ""
            for value in info_text.split("\n"):
                if len(tmp + value+"\n") <= 4096:
                    tmp += value+"\n"
                else:
                    arrayMsg.append(tmp)
                    tmp = value+"\n"
            arrayMsg.append(tmp)
            for x in range(len(arrayMsg)):
                if x + 1 == len(arrayMsg):
                    msg = await callback_query.message.answer(arrayMsg[x], reply_markup=reply_markup)
                else:
                    await callback_query.message.answer(arrayMsg[x])
            await state.update_data({"last_msg": msg.message_id})
            await AdminStates.change_tariff.set()
            await state.update_data({"ct_state": "ct_delete_channel"})
        except Exception as e:
            print(e)
        return
    msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.change_tariff.set()
