from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.admin import AdminStates
from models import User


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
    ct_state = data.get("s_state")
    text = msg.text
    info_text = ""
    reply_markup = keyboard.state_keyboard(is_back=True)
    if ct_state == "max_connection":
        if text.isdigit():
            info_text = f"Вы ввели максимальное кол-во подключений:\n{text}\nВсе верно?"
            await state.update_data({"s_max_connection": text})
            reply_markup = keyboard.state_keyboard(is_back=True, is_yes=True)
        else:
            info_text = f"Вы ввели не число, введите целое число, пожалуйста"

    arrayMsg = []
    tmp = ""
    for value in info_text.split("\n\n"):
        if len(tmp + value + "\n\n") <= 4096:
            tmp += value + "\n\n"
        else:
            arrayMsg.append(tmp)
            tmp = value + "\n\n"
    arrayMsg.append(tmp)
    for x in range(len(arrayMsg)):
        if x + 1 == len(arrayMsg):
            msg = await msg.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML, reply_markup=reply_markup)
        else:
            await msg.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML, )
    await state.update_data({"last_msg": msg.message_id})
    await AdminStates.change_settings_check_input.set()
