import Header from "./components/Header/Header";
import Footer from "./components/Footer/Footer";
import Main from "./components/Main/Main";

function App() {
    return (
        <div className="cover-container d-flex w-100 h-100 p-3 mx-auto flex-column">
            <Header/>
            <Main/>
            <Footer/>
        </div>

    );
}

export default App;
