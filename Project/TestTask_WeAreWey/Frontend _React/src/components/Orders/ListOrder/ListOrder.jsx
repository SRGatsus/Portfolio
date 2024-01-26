import {Link} from "react-router-dom";
import React, {useState} from "react";
import NewOrder from "../../Modal/NewOrder/NewOrder";


function ListOrder(props) {
    const [activeModalCreateOrder, setActiveModalCreateOrder] = useState(false);

    function handleSubmitCreateOrder(event) {
        setActiveModalCreateOrder(false);
        props.handleSubmitCreateOrder();

    }

    return (
        <div>
            <div style={{backgroundColor: "darkgrey"}} className="mt-5 mb-5 rounded">
                <div className="border-bottom hstack row g-0 p-3 ">
                    <div className="col-sm-6 col-md-10 fs-1" align="left">Список заказов</div>
                    <button className="col-6 col-md-2  btn btn-primary btn-secondary btn-outline-dark"
                            aria-current="page" onClick={() => setActiveModalCreateOrder(true)}
                    > Создать заказ
                    </button>
                </div>
                {props.listOrder.length === 0 ?
                    <div className="p-5">
                        <div className="card-body p-5">

                            <p className="card-text fs-4 p-5" align="center">
                                Ой список заказов пуст, создайте новый
                            </p>
                        </div>
                    </div>
                    :
                    <ul className="list-unstyled">
                        {props.listOrder.map((item, idx) => (
                            <li key={'menu-header-list-' + item.Order_id}>
                                <div className="card bg-secondary rounded m-3">
                                    <div className="card-title fs-3  border-bottom m-3 "
                                         align="left">
                                        <div className="hstack gap-2 ">
                                            <div className="col-sm-6 col-md-10 fs-1"
                                                 align="left">{item.Title} №{item.Order_id}</div>
                                            <Link to={"/order/" + item.Order_id}
                                                  className="col-6 col-md-2 btn btn-primary btn btn-secondary btn btn-outline-dark"
                                                  aria-current="page"
                                            > Изменить заказ</Link>
                                        </div>

                                    </div>
                                    <div className="card-body">

                                        <p className="card-text fs-4" align="left">
                                            Cумма: {item.Sum} руб. <br/>
                                            Дата заказа: {item.Date}
                                        </p>
                                    </div>
                                </div>
                            </li>
                        ))}
                    </ul>}

            </div>
            <NewOrder active={activeModalCreateOrder} setActive={setActiveModalCreateOrder}
                      handleSubmit={handleSubmitCreateOrder}/>
        </div>
    );
}

export default ListOrder;