import { useState } from "react"
// import logolight from "/logos/logo-light.png"
// import logodark from "/logos/logo-dark.png"
// import horizontallogolight from "/logos/horizontal-logo-light.png"
// import horizontallogodark from "/logos/horizontal-logo-dark.png"
// import logoiconlight from "/logos/logo-icon-light.png"
// import logoicondark from "/logos/logo-icon-dark.png"
import llogoicondarktransparent from "/logos/logo-icon-dark-transparent.png"
import llogoiconlighttransparent from "/logos/logo-icon-light-transparent.png"
import Login from "./pages/Login"

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      <div className="w-full h-full">
        <Login 
          logolighttransparent={llogoiconlighttransparent}
        />
      </div>
    </>
  )
}

export default App
