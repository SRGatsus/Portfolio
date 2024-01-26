import React from "react";
import {navigationBar} from "../Header/NavigationBar";
import {Link} from "react-router-dom";
import './Footer.css'

const Footer = () => {
    return (
        <footer className=" mt-auto text-white-500 border-top">
            <ul className="nav justify-content-center pt-3 pb-3 ">
                {navigationBar.map((item, idx) => (
                    <li className="nav-item" key={'menu-footer-list-' + idx}>
                        <Link to={item.Link} className="nav-link px-2 text-muted active "
                              aria-current="page">
                            {item.Title}
                        </Link>
                    </li>
                ))}
            </ul>
            <p className="text-center text-muted">© 2022 Company, Inc</p>
        </footer>

    )
}

export default Footer;