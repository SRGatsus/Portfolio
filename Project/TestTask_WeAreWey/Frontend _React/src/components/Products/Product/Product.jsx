import React, {useState} from "react";

function CalculateSum(order) {
    order.Sum = 0;
    order.Product_Order.map((item) => {
        order.Sum += item.Price * item.CountProduct;
        return false
    })
    return order;
}

function removeProductInList(itemProduct, order) {
    var i = -1;
    order.Product_Order.map((item, index) => {
        if (itemProduct.Product_id === item.Product_id) {
            i = index;
        }
        return false
    })
    order.Product_Order.splice(i, 1);
}

function ChangingQuantityProduct(item, step, order) {

    if (parseInt(item.CountProduct) === 1 && parseInt(step) === -1) {
        if (window.confirm("Вы точно ходите удалить продукт?")) {
            removeProductInList(item, order)
        }
    } else {
        item.CountProduct = parseInt(item.CountProduct) + parseInt(step);
    }
    return item;
}


function Product({item, setSum, order}) {
    const [itemCountProduct, setItemCountProduct] = useState(item.CountProduct);
    return (
        <div key={'menu-header-list-' + item.Product_id}>
            <div className="card bg-secondary rounded m-3">
                <div className="card-title fs-3  border-bottom m-3  " align="left">
                    <div className="hstack row g-0 p-3  ">
                        <div className="col-sm-6 col-md-11 fs-1" align="left">
                            Товар №{item.Product_id}
                        </div>
                        <button
                            className="col-6 col-md-1  btn-close btn btn-primary btn-secondary btn-outline-dark"

                            aria-label="Закрыть" onClick={() => {
                            if (window.confirm("Вы точно ходите удалить продукт?")) {
                                removeProductInList(item, order);
                                setSum(CalculateSum(order).Sum);
                            }
                        }}
                        >
                        </button>
                    </div>

                </div>
                <div className="card-body  hstack row g-0 p-3">
                    <div className="col-sm-6 col-md-10 fs-3" align="left">
                        <p className="card-text fs-4" align="left">
                            Название: {item.Title}<br/>
                            Описание: {item.Description}<br/>
                            Стоимость: {item.Price} руб.
                        </p>
                    </div>
                    <div className="col-6 col-md-2 justify-content-center fs-4" align="center"
                         aria-current="page">
                        Количество
                        <div className="container-input-count">
                                                <span className="next"
                                                      onClick={() => {
                                                          setItemCountProduct(ChangingQuantityProduct(item, +1, order).CountProduct)
                                                          setSum(CalculateSum(order).Sum);
                                                      }}></span>
                            <span className="prev"
                                  onClick={() => {
                                      setItemCountProduct(ChangingQuantityProduct(item, -1, order).CountProduct);
                                      setSum(CalculateSum(order).Sum);
                                  }}></span>
                            <div className="box ">
                                <span>{itemCountProduct}</span>
                            </div>
                        </div>
                        <div/>
                    </div>
                </div>
            </div>
        </div>

    );
}

export default Product;