import React from "react";
import Product from "./Product/Product";


function ListProduct({order, setSum}) {

    return (<ul className="list-unstyled">
            {order.Product_Order.length !== 0 ? order.Product_Order.map((item, idx) => (
                <li key={'menu-header-list-' + item.Product_id}>
                    <Product item={item} setSum={setSum} order={order}/>
                </li>
            )) : <div className="p-5">
                <div className="card-body p-5">

                    <p className="card-text fs-4 p-5" align="center">
                        Вы еще не добавили товар в заказа
                    </p>
                </div>
            </div>}
        </ul>

    );
}

export default ListProduct;