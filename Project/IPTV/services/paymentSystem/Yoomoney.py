import datetime
from .IPaymentSystem import IPaymentSystem
import os
import requests
from yoomoney import Client, Authorize, Quickpay, Operation


class Yoomoney(IPaymentSystem):
    number
    def __init__(self):
        super().__init__(False, os.environ["BASE_URL_YOOMONEY_API"], "Yoomoney")

    def quickpay(self, label, user_id, sum, text) -> str:
        quick = Quickpay(
            receiver=os.environ["NUMBER_ACCOUNT"],
            quickpay_form="shop",
            comment=label,
            targets=text,
            paymentType="SB",
            sum=sum,
            label=label
        )
        return quick.redirected_url

    def checkQuickpay(self, label) -> int | Operation:
        client = Client(os.environ["TOKEN_YOOMONEY"])
        history = client.operation_history(label=label)
        for operation in history.operations:
            print()
            print("Operation:", operation.operation_id)
            print("\tStatus     -->", operation.status)
            print("\tDatetime   -->", operation.datetime)
            print("\tTitle      -->", operation.title)
            print("\tPattern id -->", operation.pattern_id)
            print("\tDirection  -->", operation.direction)
            print("\tAmount     -->", operation.amount)
            print("\tLabel      -->", operation.label)
            print("\tType       -->", operation.type)
        if (len(history.operations) == 0):
            return 0
        return history.operations[0]
