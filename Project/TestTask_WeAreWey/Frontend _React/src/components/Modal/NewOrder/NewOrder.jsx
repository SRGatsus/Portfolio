import React from "react";
import "../Modal.css";


function NewOrder({active, setActive, handleSubmit}) {
    return (
        <div
            className={active ? "modal active modal-signin p-5" : "modal"}
            tabIndex="-2424" role="dialog"
            onClick={() => setActive(false)}
            id="modalSheet">
            <div className="modal-dialog " role="document">
                <div className="modal-content rounded-4 shadow" onClick={e => e.stopPropagation()}>
                    <div className="modal-header border-bottom-0 ">
                        <h5 className="modal-title text-dark m-3">Создание нового заказа</h5>
                        <button type="button" className="btn-close  m-1" data-bs-dismiss="modal"
                                onClick={() => setActive(false)}
                                aria-label="Закрыть"></button>
                    </div>
                    <div className="modal-body py-0">
                        <form className="" onSubmit={handleSubmit}>
                            <div className="form-floating mb-3 text-dark">
                                <input type="text" className="form-control rounded-3 text-dark"
                                       id="validationDefaultName"
                                       placeholder="Shop"/>
                                <label htmlFor="floatingInput">Название магазина</label>
                            </div>
                        </form>
                    </div>
                    <div className="modal-footer flex-column border-top-0">
                        <button type="submit" className="button rounded w-100 mx-0 mb-2"
                                onClick={handleSubmit}>Создать
                        </button>
                        <button type="button" className="btn btn-lg btn-light w-100 mx-0"
                                onClick={() => setActive(false)}
                                data-bs-dismiss="modal">Отмена
                        </button>
                    </div>
                </div>
            </div>
        </div>

    );
}

export default NewOrder;
