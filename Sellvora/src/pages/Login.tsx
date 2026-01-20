import React,{ useState } from 'react'
import Customerlogin from '../components/customerlogin.tsx'

type LoginProps = {
  logolighttransparent: string
}
const Login = ({ logolighttransparent }: LoginProps) => {
  const[active,setActive] = useState<'Customer' | 'Admin'>('Customer')
  const baseClass =
    'cursor-pointer bg-[#f1f5f9] text-[#64748B] border-[#cbd5e1] m-auto w-1/2 text-center text-2xl py-4 rounded-md shadow-md'
  const activeClass =
    'bg-blue-950 text-white shadow-xl'
    
    return (
    <div className='w-screen h-screen flex-col justify-center items-center bg-[#f1f5f9]'>
      {/* Logo with tagline */}
      <div className='w-screen flex-col justify-center items-center text-center'>
        <img src={logolighttransparent} alt="Logo light" className='w-1/5 m-auto p-auto'/>
        <h5 className='text-[#64748B] text-sm'>End-to-End Online Selling Solution</h5>
      </div>
      {/* Login Card */}
      <div className='bg-[#ffffff] w-auto h-auto m-auto rounded-md mt-5 ml-1'>
        {/* Login Selector */}
        <div className='flex justify-between gap-2 h-full w-full m-auto pt-2 px-2'>
          {/* Customer */}
          <div id="customer" onClick={() => setActive('Customer')} className={`${baseClass} ${active === 'Customer' ? activeClass : ''}`}>
              Customer Login
          </div>
          {/* Admin */}
          <div id="admin" onClick={() => setActive('Admin')} className={`${baseClass} ${active === 'Admin' ? activeClass : ''}`}>
              Admin Login
          </div>
        </div>
        <div className='pb-3'>
          <Customerlogin />
        </div>
        
      </div>
    </div>
  )
}
export default Login
