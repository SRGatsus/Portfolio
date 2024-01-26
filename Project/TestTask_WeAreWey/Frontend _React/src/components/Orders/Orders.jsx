import Header from "../Header/Header";
import Footer from "../Footer/Footer";
import ListOrder from "./ListOrder/ListOrder";
import React, {useEffect, useState} from "react";
import axios from "axios";


async function GetOrders() {
    var response = await axios.get(process.env.REACT_APP_API_URL + '/orders');
    return response.data;
}

async function CreateOrder(Title) {
    var req = {};
    req["Title"] = Title;
    var jsonData = JSON.stringify(req)
    var response = await axios.post(process.env.REACT_APP_API_URL + '/newOrder',jsonData,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        timeout: 10000,
    });
    return response.data;
}

function Orders() {
    const [orders, setOrders] = useState(null);

    async function handleSubmitCreateOrder(event) {
        var Title = document.getElementById("validationDefaultName").value;
        setOrders(await CreateOrder(Title));
    }

    useEffect(() => {
        (async () => {
            setOrders(await GetOrders());
        })()
    }, []);

    return (
        <div className="cover-container d-flex w-100 h-100 p-3 mx-auto flex-column">
            <Header/>
            {orders ? <ListOrder listOrder={orders} handleSubmitCreateOrder={handleSubmitCreateOrder}/> :
                <div className="d-flex justify-content-center">
                    <div className="spinner-border text-danger " role="status">
                        <span className="visually-hidden">Загрузка...</span>
                    </div>
                </div>
            }
            <Footer/>

        </div>

    );
}

export default Orders;
