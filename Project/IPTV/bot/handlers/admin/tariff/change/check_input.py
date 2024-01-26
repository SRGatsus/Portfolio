from aiogram import types
from aiogram.dispatcher import FSMContext
from ipytv import playlist
from ipytv.channel import IPTVChannel

import keyboard
from loader import bot, db_session
from models import User, Tariff
from bot.states.admin import AdminStates


def isNumber(s):
    try:
        float(s)
        return True
    except ValueError:
        return False


async def check_input(msg: types.Message, state: FSMContext, user: User):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                               is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("ct_state")
    text = msg.text
    info_text = ""
    reply_markup = keyboard.state_keyboard(is_back=True)
    if ct_state == "ct_set_name":
        if 4 <= len(text) <= 120:
            info_text = f"Вы ввели название тарифа:\n{text}\nВсе верно?"
            await state.update_data({"ct_name": text})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели текст меньше 4 символов или больше 120.\nТекущая длинна: {len(text)}\nВведите заново текст"
    elif ct_state == "ct_set_description":
        if 30 <= len(text) <= 1200:
            info_text = f"Вы ввели описание тарифа:\n{text}\nВсе верно?"
            await state.update_data({"ct_description": text})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели текст меньше 30 символов или больше 1200.\nТекущая длинна: {len(text)}\nВведите заново текст"
    elif ct_state == "ct_set_price":
        if isNumber(text) and float(text) >= 2:
            price = float(text)
            info_text = f"Вы ввели цену тарифа:\n{price}\nВсе верно?"
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
            await state.update_data({"ct_price": price})
        else:
            info_text = f"Вы ввели цену меньше 2.\n Введите заново цена"
    elif ct_state == "ct_set_days":
        if text.isdigit() and int(text) >= 2:
            price = int(text)
            info_text = f"Вы ввели кол-во дней действия тарифа:\n{price}\nВсе верно?"
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
            await state.update_data({"ct_days": price})
        else:
            info_text = f"Вы ввели число меньше 2 или ввели не целое.\n Введите заново цену"
    elif ct_state == "ct_set_list_channel":
        document = msg.document
        if document:
            file_expansion = document.file_name.split(".")[-1]
            if file_expansion in ["php", "m3u", "m3u8", "txt"]:
                info_text = "Идет обработка файла, подождите чуть чуть"
                msg = await msg.answer(info_text)
                file = await bot.download_file_by_id(document.file_id)
                data_file = [value.decode("utf-8-sig") for value in file.readlines()]
                try:
                    pl = playlist.loadl(data_file)
                    info_text = ("Файл был обработан\n"
                                 f"Файле нашлось {len(pl.get_channels())} канал\ов\n")
                    unique_numbers = list(set([channel.attributes.get('group-title') for channel in pl.get_channels()]))
                    info_text += f"Нашли {len(unique_numbers)} группу\ы\n"
                    for value in unique_numbers:
                        info_text += f"{value}\n"
                    info_text += f"\nВсе верно?"
                    reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
                    for x in range(0, len(info_text), 4096):
                        if x + 4097 > len(info_text):
                            msg = await msg.answer(info_text[x:x + 4096], reply_markup=reply_markup)
                        else:
                            await msg.answer(info_text[x:x + 4096])
                    await state.update_data({"last_msg": msg.message_id, "ct_list_channel": str(pl)})
                    await AdminStates.change_tariff_check_input.set()
                    return
                except Exception as e:
                    info_text = (
                        f"Ой, что-то пошло не так\n"
                        f"Загрузите файл еще раз\n"
                        f"{e}"
                    )
                    await msg.edit_text(info_text)
                    return

            else:
                info_text = "Вы загрузили не то"
        else:
            info_text = "Вы не отправили докумен, отправте пожалуйста документ"
    elif ct_state == "ct_custom_channel":
        list_arg = text.split(";")
        if len(list_arg) >= 2:
            try:
                name = list_arg[0]
                url = list_arg[1]
                attributes = {}
                for value in list_arg[2:]:
                    attributes[value.split("=")[0]] = value.split("=")[1]
                chanel = IPTVChannel(name=name, url=url, attributes=attributes)
                info_text = f"Вы хотите добавить канал:\n{chanel.to_m3u_plus_playlist_entry()}"
                reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
                await state.update_data({"ct_custom_chanel": str(chanel.to_m3u_plus_playlist_entry())})
            except Exception as e:
                info_text = (
                    f"Ой, что-то пошло не так\n"
                    f"Загрузите файл еще раз\n"
                    f"{e}"
                )
                await msg.answer(info_text)
                return
        else:
            info_text = f"Выввели неверный формат"
    elif ct_state == "ct_delete_channel":
        if text.isdigit() and int(text) >= 0:
            id_channel = int(text)
            await msg.answer("Подождите чуть чуть, ищем канал")
            tariff = db_session.query(Tariff).filter(Tariff.id == data.get("t_id")).one()
            pl = playlist.loads(tariff.list_channel)
            if id_channel < len(pl.get_channels()):
                info_text = f"Вы хотите удалить канал:\n{pl.get_channels()[id_channel].name}"
                reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
                await state.update_data({"ct_delete_channel": id_channel})
            else:
                info_text = f"Вы ввели число больше {len(pl.get_channels()) - 1} или ввели не целое.\n Введите заново номер канала"

        else:
            info_text = f"Вы ввели число меньше 0 или ввели не целое.\n Введите заново номер канала"
    else:
        info_text = f"Все верно?"
        reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
    msg = await msg.answer(info_text, parse_mode=types.ParseMode.HTML,
                           reply_markup=reply_markup)
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.change_tariff_check_input.set()
