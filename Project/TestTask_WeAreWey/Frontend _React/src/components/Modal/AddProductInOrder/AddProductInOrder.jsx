import React, {useEffect, useState} from "react";
import "../Modal.css";
import axios from "axios";


function addProductInList(itemProduct, order) {
    itemProduct["CountProduct"] = 1;
    var orders = Object.assign({}, order)
    orders.Product_Order = [...order.Product_Order, itemProduct]
    return orders;
}

function CalculateSum(order) {
    order.Sum = 0;
    order.Product_Order.map((item) => {
        return order.Sum += item.Price * item.CountProduct;
    })
    return order;
}

function UpdateList(order, products) {
    if (order) {
        order.Product_Order.map((itemListOrder, index1) => {
            var i = -1;
            products.map((item, index2) => {
                if (item.Product_id === itemListOrder.Product_id) {
                    i = index2;
                }
                return false
            })
            if (i !== -1) {
                products.splice(i, 1);
                i = -1;
            }
            return false
        })
    }

    return products;
}

async function GetProducts(active) {
    if (active) {
        var response = await axios.get(process.env.REACT_APP_API_URL + '/products');
        return response.data;
    }
    return [];
}

function AddProductInOrder({active, setActive, handleSubmit, order, setSum, setOrder}) {
    const [products, setProducts] = useState(null);


    useEffect(() => {
        (async () => {
            var products = UpdateList(order, await GetProducts(active));
            setProducts(products);
        })()
    }, [active]);
    return (
        <div
            className={active ? "modal active modal-signin p-5" : "modal"}
            tabIndex="-2424" role="dialog"
            onClick={() => setActive(false)}
            id="modalSheet">
            <div className="modal-dialog " role="document" style={{maxWidth: "90%", height: "75%"}}>


                <div className="modal-content rounded-4 shadow" onClick={e => e.stopPropagation()}>
                    <div className="modal-header border-bottom-0 ">
                        <h5 className="modal-title text-dark m-3">Список продуктов</h5>
                        <button type="button" className="btn-close  m-1" data-bs-dismiss="modal"
                                onClick={() => setActive(false)}
                                aria-label="Закрыть"></button>
                    </div>
                    <div className="modal-body py-0">
                        {products ?
                            <ul className="list-unstyled">
                                {products.length !== 0 ? products.map((item, idx) => (
                                        <li key={'list-product-' + item.Product_id}>
                                            <div className="card bg-secondary rounded m-3">
                                                <div className="card-title fs-3  border-bottom m-3 " align="left">
                                                    <div className="hstack gap-2 ">
                                                        <div className="col-sm-6 col-md-10 fs-1" align="left">
                                                            Товар №{item.Product_id}
                                                        </div>
                                                        <button
                                                            className="col-6 col-md-2  btn btn-primary btn-secondary btn-outline-dark"
                                                            aria-current="page" onClick={() => {
                                                            var orderNew = CalculateSum(addProductInList(item, order))
                                                            setSum(orderNew.Sum);
                                                            setProducts(UpdateList(orderNew, products))
                                                            setOrder(orderNew);
                                                        }}
                                                        > Добавить
                                                        </button>
                                                    </div>


                                                </div>
                                                <div className="card-body  hstack row g-0 p-3">
                                                    <div className="col-sm-6 col-md-10 fs-3" align="left">
                                                        <p className="card-text fs-4" align="left">
                                                            Название: {item.Title}<br/>
                                                            Стоимость: {item.Price} руб.
                                                        </p>
                                                    </div>
                                                </div>
                                            </div>
                                        </li>
                                    )) :
                                    <div className="p-5">
                                        <div className="card-body p-5">

                                            <p className="card-text fs-4 p-5 text-dark" align="center">
                                                Ой товаров нет, либо вы добавили все в заказ
                                            </p>
                                        </div>
                                    </div>}
                            </ul> :
                            <div className="p-5">
                                <div className="card-body p-5">

                                    <p className="card-text fs-4 p-5" align="center">
                                        Загрузка.
                                    </p>
                                </div>
                            </div>}
                    </div>
                </div>
            </div>
        </div>

    );
}

export default AddProductInOrder;
