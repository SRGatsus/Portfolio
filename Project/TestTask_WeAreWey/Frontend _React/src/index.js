import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import reportWebVitals from './reportWebVitals';
import 'bootstrap/dist/css/bootstrap.css';
import 'bootstrap/dist/js/bootstrap';
import Orders from "./components/Orders/Orders";
import {BrowserRouter, Route, Routes,} from 'react-router-dom';
import Order from "./components/Order/Order";

const root = ReactDOM.createRoot(document.getElementById('root'));

root.render(
    <BrowserRouter>
        <Routes>
            <Route path="/" element={<App/>}/>
            <Route path="/orders" element={<Orders/>}/>
            {/*<Route path="/product" element={<Orders/>}/>*/}
            <Route path="/order/:id" element={<Order/>}/>
        </Routes>
    </BrowserRouter>,
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();
