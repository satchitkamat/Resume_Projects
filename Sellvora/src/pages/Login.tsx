import React,{ useState } from 'react'
import CustomerLogin from '../components/Customerlogin.tsx'
import AdminLogin from '../components/Adminlogin.tsx'

type LoginProps = {
  logolighttransparent: string
}
const Login = ({ logolighttransparent }: LoginProps) => {
  const[active,setActive] = useState<'Customer' | 'Admin'>('Customer')
  const baseClass =
    'cursor-pointer border-sellvora-border m-auto w-1/2 text-center text-2xl py-4 rounded-md shadow-md lg:rounded-none'    
    return (
    <div className='w-screen h-screen flex-col justify-center items-center bg-sellvora-page lg:flex-row lg:min-h-screen'>
      {/* Logo with tagline */}
      <div className='w-screen flex-col justify-center items-center text-center'>
        <img
          src={logolighttransparent}
          alt="Sellvora logo"
          className="w-24 md:w-32 lg:w-40 mx-auto mb-4"
        />

        <h5 className='text-sellvora-textSecondary text-sm'>End-to-End Online Selling Solution</h5>
      </div>
      {/* Login Card */}
      <div className='w-auto h-auto m-auto rounded-md mt-5'>
        {/* Login Selector */}
        <div className='flex justify-between gap-2 h-full w-full m-auto pt-2 lg:gap-0 '>
          {/* Customer */}
          <div id="customer" onClick={() => setActive('Customer')} className={`${baseClass} ${active === 'Customer' ? 'bg-sellvora-darkblue text-sellvora-surface shadow-xl' : 'bg-sellvora-page text-sellvora-textSecondary'}`}>
              Customer Login
          </div>
          {/* Admin */}
          <div id="admin" onClick={() => setActive('Admin')} className={`${baseClass} ${active === 'Admin' ? 'bg-sellvora-darkblue text-sellvora-surface shadow-xl' : 'bg-sellvora-page text-sellvora-textSecondary'}`}>
              Admin Login
          </div>
    
        </div>
        <div className="flex flex-col lg:flex-row min-h-min">

          {/* LEFT — Customer */}
          <div
            className={`w-full lg:w-1/2 ${
              active === 'Customer' ?'block lg:bg-transparent' : 'hidden lg:block lg:bg-sellvora-darkblue'
            }`}
          >
            <CustomerLogin Cactive={active === 'Customer'}/>
          </div>
          
          {/* RIGHT — Admin */}
          <div
            className={`w-full lg:w-1/2 lg:h-max ${
              active === 'Admin' ? 'block lg:bg-tranparent' : 'hidden lg:block lg:bg-sellvora-darkblue'
            }`}
          >
            <AdminLogin Aactive={active === 'Admin'}/>
          </div>
          
        </div>

      </div>
    </div>
  )
}
export default Login
