class IPaymentSystem:
    token = ""
    baseUrl = ""
    name = "PaymentSystem"

    def __init__(self, token, baseUrl, name):
        self.token = token
        self.baseUrl = baseUrl
        self.name = name

    def main(self):
        return 0

    def quickpay(self,label,user_id,sum,text):
        return 0

