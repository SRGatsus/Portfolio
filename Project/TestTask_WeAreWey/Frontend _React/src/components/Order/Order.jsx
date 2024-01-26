import Header from "../Header/Header";
import Footer from "../Footer/Footer";
import React, {useEffect, useState} from "react";
import './Order.css';
import axios from "axios";
import ListProduct from "../Products/ListProduct";
import AddProductInOrder from "../Modal/AddProductInOrder/AddProductInOrder";
import {useNavigate} from "react-router";

function CalculateSum(order) {
    order.Sum = 0;
    order.Product_Order.map((item) => {
        order.Sum += item.Price * item.CountProduct;
        return false
    })
    return order;
}

async function GetIdOrder(id) {
    var req = {};
    req["Order_id"] = id;
    var jsonData = JSON.stringify(req)
    var response = await axios.post(process.env.REACT_APP_API_URL + '/order',jsonData,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        timeout: 10000,
    });
    return CalculateSum(response.data[0]);
}

async function SaveOrder(order, navigate) {
    var req = {};
    req["Order_id"] = order.Order_id;
    req["Product_Order"] = [];

    order.Product_Order.map((item) => {
        var Product_Order = {};
        Product_Order["CountProduct"] = item.CountProduct;
        Product_Order["Product_id"] = item.Product_id;
        Product_Order["Price"] = item.Price;
        req["Product_Order"].push(Product_Order);
        return false
    })
    axios.defaults.headers.post['Content-Type'] = 'application/x-www-form-urlencoded';
    var jsonData = JSON.stringify(req);
    await axios.post(process.env.REACT_APP_API_URL + '/orderer', jsonData,{
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        timeout: 10000,
    }).then((res,req) =>{
        console.log("Все супер")
    });

    navigate(`/orders`);
    return [];
}

function Orders() {
    const [modalNewOrderActive, setModalNewOrderActive] = useState(false);
    const [order, setOrder] = useState(null);
    const [sum, setSum] = useState(0);
    const navigate = useNavigate();

    function handleSubmit(event) {
        setModalNewOrderActive(false);
    }

    useEffect(() => {
        (async () => {
            var id = window.location.pathname.split("/")[2];
            var orderNew = await GetIdOrder(id);
            setOrder(orderNew);
            setSum(orderNew.Sum);
        })()
    }, []);


    return (
        <div className="cover-container d-flex w-100 h-100 p-3 mx-auto flex-column">
            <Header/>
            {order ?
                order.isNotOrder ?
                    <div>Ой такого заказа нет</div>
                    :
                    <div>
                        <div style={{backgroundColor: "darkgrey"}} className="mt-5 mb-5 rounded">
                            <div className="border-bottom hstack row g-0 p-3 ">
                                <div className="col-sm-6 col-md-10 fs-1"
                                     align="left">{order.Title} №{order.Order_id}</div>
                                <button className="col-6 col-md-2  btn btn-primary btn-secondary btn-outline-dark"
                                        aria-current="page" onClick={() => setModalNewOrderActive(true)}
                                > Добавить товар
                                </button>
                            </div>
                            <ListProduct order={order} setSum={setSum}/>
                            <div className="border-top hstack row g-0 p-3 ">

                                <div className="col-sm-6 col-md-10 fs-3" align="left">Сумма
                                    заказа: {sum}</div>
                                <button className="col-6 col-md-2  btn btn-primary btn-secondary btn-outline-dark"
                                        aria-current="page" onClick={() => {
                                    SaveOrder(order, navigate);
                                }}
                                > Сохранить изменения
                                </button>
                            </div>
                        </div>
                    </div>
                :
                <div className="d-flex justify-content-center">
                    <div className="spinner-border text-danger " role="status">
                        <span className="visually-hidden">Загрузка...</span>
                    </div>
                </div>
            }

            <Footer/>
            <AddProductInOrder active={modalNewOrderActive} setActive={setModalNewOrderActive} order={order}
                               setSum={setSum} setOrder={setOrder}
                               handleSubmit={handleSubmit}/>
        </div>

    );
}


export default Orders;