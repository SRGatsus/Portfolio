from aiogram import types
from aiogram.dispatcher import FSMContext

import keyboard
from bot.states.admin import AdminStates
from loader import bot, db_session
from models import User, Tariff, Transaction
from models.transaction import typesTransaction


async def again_input(callback_query: types.CallbackQuery, state: FSMContext, user: User):
    data = await state.get_data()
    flag=True
    if not user.is_admin:
        info_text = f"Вы не админ"
        msg = await callback_query.message.answer(info_text, reply_markup=keyboard.state_keyboard(is_main_menu=True,
                                                                                                  is_back=False))
        await state.update_data({"last_msg": msg.message_id})
        return
    ct_state = data.get("cu_state")
    info_text = ""
    if ct_state == "cu_add_tariff":
        tariffs = db_session.query(Tariff).all()
        info_text = (f"Введите номер тарифа для подробной информации о нем\n"
                     f"Обозначения:\n"
                     f"🟢 - тариф активен\n"
                     f"🟡 - тариф неактивен, но есть пользователь пользующийся им\n"
                     f"🔴 - тариф неактивен\n"
                     f"Список тарифов:\n")
        if len(tariffs) == 0:
            info_text += f"Пусто\n"
        else:
            for tariff in tariffs:
                if tariff.is_active:
                    info_text += f"🟢 {tariff.id}) {tariff.title}\n"
                else:
                    is_user = db_session.query(User).filter(User.tariff_id == tariff.id).count()
                    if is_user > 0:
                        info_text += f"🟡 {tariff.id}) {tariff.title}\n"
                    else:
                        info_text += f"🔴 {tariff.id}) {tariff.title}\n"
        info_text += ("Введите номер тарифа,который желаете добавить:")
        await state.update_data({"cu_state": "cu_add_tariff"})
    elif ct_state == "cu_change_tariff":
        tariffs = db_session.query(Tariff).all()
        info_text = (f"Введите номер тарифа для подробной информации о нем\n"
                     f"Обозначения:\n"
                     f"🟢 - тариф активен\n"
                     f"🟡 - тариф неактивен, но есть пользователь пользующийся им\n"
                     f"🔴 - тариф неактивен\n"
                     f"Список тарифов:\n")
        if len(tariffs) == 0:
            info_text += f"Пусто\n"
        else:
            for tariff in tariffs:
                if tariff.is_active:
                    info_text += f"🟢 {tariff.id}) {tariff.title}\n"
                else:
                    is_user = db_session.query(User).filter(User.tariff_id == tariff.id).count()
                    if is_user > 0:
                        info_text += f"🟡 {tariff.id}) {tariff.title}\n"
                    else:
                        info_text += f"🔴 {tariff.id}) {tariff.title}\n"
        info_text += (
            "Введите номер тарифа,на который хотите поменять(время дейтсвия тарифа останеться тем же, что была ранее):")
        await state.update_data({"cu_state": "cu_change_tariff"})
    elif ct_state == "cu_blocked":
        info_text += ("Введите причину блокировки:")
        await state.update_data({"cu_state": "cu_blocked"})
    elif ct_state == "cu_list_transaction":
        await AdminStates.change_user_check_input.set()
        transactions = db_session.query(Transaction).filter(Transaction.user_id == data.get("u_id")).order_by(
            Transaction.id.desc()).all()
        try:
            for transaction in transactions:
                info_text += f"{transaction.id}) {typesTransaction.get(transaction.label.split(':')[0], 'Не известный тип')}\n"
        except Exception as e:
            print(e)
        info_text += "Введите номер транзакции для подробной информации о нем\n"
        await state.update_data({"cu_state": "cu_list_transaction"})
    arrayMsg = []
    tmp = ""
    for value in info_text.split("\n"):
        if len(tmp + value + "\n") <= 4096:
            tmp += value + "\n"
        else:
            arrayMsg.append(tmp)
            tmp = value + "\n"
    arrayMsg.append(tmp)
    for x in range(len(arrayMsg)):
        if x + 1 == len(arrayMsg):
            msg = await callback_query.message.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML,
                                                      reply_markup=keyboard.state_keyboard(is_back=True))
        else:
            await callback_query.message.answer(arrayMsg[x], parse_mode=types.ParseMode.HTML)
    await state.update_data({"last_msg": msg.message_id})
    if flag:
        await AdminStates.change_user.set()
