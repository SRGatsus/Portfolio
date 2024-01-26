from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.admin import AdminStates
from loader import bot, db_session
from models import User, Transaction, Connection, Tariff


async def view(msg: types.Message, state: FSMContext, user: User, admin_id=None):
    data = await state.get_data()
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                               is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    try:
        admin_id = admin_id or msg.text
        user_value = db_session.query(User).filter(User.id == (admin_id)).one()
        if user_value:
            await AdminStates.view_user.set()
            info_text = str(user_value) + "\n"
            if user_value.tariff_id > -1:
                tariff = db_session.query(Tariff).filter(Tariff.id == user_value.tariff_id).one()
                status = "неактивен"
                if user_value.tariff_status == 1:
                    status = "активен"
                elif user_value.tariff_status == 2:
                    status = "отключен"
                info_text += (f"Имеется тариф:\n{tariff}"
                              f"Статус: {status}\n"
                              f"Дата подключение: {(user_value.tariff_start).strftime('%d-%m-%Y в %H:%M')}\n"
                              f"Дата окончания: {(user_value.tariff_end).strftime('%d-%m-%Y в %H:%M')}\n")
                info_text += (
                    "\n\n'Изменить тариф' - изменяет тариф, срок дейтсвия тарифа не меняется"
                    "'Продлить тариф' - продливает действия тарифа,если срок дейтсвия истек, то назначает новый по принципу текущая дата + срок дейтсвия тарифа\n"
                    "'Отключить тариф' - отключает тариф, тариф остается у пользователя, но он не сможет пользоваться им\n"
                    "'Включить тариф' - включает тариф, срок дейтсвия тарифа не изменяется\n"
                    "'Удалить тариф' - полностью удаляет тариф у пользвотеля\n"
                    "'Заблокировать' - блокирует функционал бота и сервиса для пользователя")

            is_transaction = (db_session.query(Transaction).filter(Transaction.user_id == admin_id).count()) > 0
            is_connection = (db_session.query(Connection).filter(Connection.user_id == admin_id).count()) > 0
            reply_markup = keyboard.admin_user_view(is_tariff=user_value.tariff_id > -1,
                                                    is_transaction=is_transaction,
                                                    is_connection=is_connection,
                                                    is_on_tariff=user_value.tariff_status == 1,
                                                    is_blocked=user_value.is_blocked,
                                                    is_admin=user_value.is_admin
                                                    )
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
                    msg = await msg.answer(arrayMsg[x], reply_markup=reply_markup)
                else:
                    await msg.answer(arrayMsg[x])
            await state.update_data({"last_msg": msg.message_id, "u_id": user_value.id})
        else:
            info_text = f"Пользователь не найден, попробуйте еще раз"
            msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
            await state.update_data({"last_msg": msg.message_id})
    except Exception as e:
        info_text = f"Пользователь не найден, попробуйте еще раз"
        msg = await msg.answer(info_text, reply_markup=keyboard.state_keyboard(is_back=True))
        await state.update_data({"last_msg": msg.message_id})
