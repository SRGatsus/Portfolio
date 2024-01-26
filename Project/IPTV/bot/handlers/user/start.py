from aiogram import types
from aiogram.dispatcher import FSMContext
from bot.states.user import UserStates
import keyboard
from services.users import checkAdmin
from loader import bot


async def start_handler(msg: types.Message|types.CallbackQuery, state: FSMContext):

    data = await state.get_data()
    await state.reset_state()
    if isinstance(msg, types.CallbackQuery):
        msg_handler = msg.message
    else:
        msg_handler = msg
    try:
        await state.reset_state()
        is_admin = checkAdmin(msg.from_user.id)
        await UserStates.main_menu.set()
        msg = await msg_handler.answer(
            "Добро пожаловать в Мир IpTV! Вашем новом надежном и стабильном источнике IPTV-сервиса.\n"
            "Мы искренне ценим ваш интерес к нашему проекту и обещаем вам высочайшее качество предоставляемых услуг.\n\n"
            "Выберите пункт меню:",
            reply_markup=keyboard.start_keyboard(is_admin),
        )
        await state.update_data({"last_msg": msg.message_id})
    except Exception as e:
        print(e)
