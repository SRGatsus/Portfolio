import React from "react";
import {navigationBar} from "./NavigationBar";
import {Link} from "react-router-dom";
import './Header.css'

const Header = () => {
    return (

        <header className="d-flex justify-content-center py-3  mb-auto border-bottom">
            <ul className="menu">
                {navigationBar.map((item, idx) => (
                    <li key={'menu-header-list-' + idx}>
                        <Link to={item.Link} className="nav-link " aria-current="page"> {item.Title}</Link>
                    </li>
                ))}
            </ul>
        </header>
    )
}

export default Header;