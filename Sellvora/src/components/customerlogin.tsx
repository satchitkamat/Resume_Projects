import React,{ useState } from 'react'

type LoginProps = {
  Cactive: boolean
}
export default function Customerlogin({ Cactive }: LoginProps) {
    const[inputActive,setInputActive] = useState<'Email' | 'Password'>()
  return (
    <>
        <div id="loginform" className={`mt-3 text-sellvora-textPrimary flex-col justify-center m-auto text-center ${Cactive == true ? 'cursor-pointer': 'pointer-events-none cursor-not-allowed'}`}>
          <h1 className='text-3xl m-auto'>Welcome Back</h1>
          <p>Welcome back! Enter your details</p>
          <div id="emaildiv" className='h-15 border-2 shadow-lg w-full max-w-sm m-auto rounded-md mt-10'>
              <p className={`${inputActive == 'Email' ? 'block' : 'hidden'} text-left pl-3`}>Enter your email.</p>
              <input type="email" id="email" className={`${inputActive == 'Email' ? 'mt-0' : 'mt-3' } bg-transparent px-2 outline-none text-2xl`} placeholder={`${inputActive == 'Email' ? '' : 'Enter your email.'}`} onClick={() => setInputActive('Email')}/>
          </div>
          <div id="passdiv" className='h-15 border-2 shadow-lg w-full max-w-sm m-auto rounded-md mt-4'>
              <p className={`${inputActive == 'Password' ? 'block' : 'hidden'} text-left pl-3`}>Enter your password.</p>
              <input type="password" id="password" className={`${inputActive == 'Password' ? 'mt-0' : 'mt-3' } bg-transparent px-2 outline-none text-2xl`} placeholder={`${inputActive == 'Password' ? '' : 'Enter your password.'}`} onClick={() => setInputActive('Password')}/>
          </div>
          <div id="action" className='mt-10 flex justify-center gap-10'>
            <div className='flex gap-1'>
              <input type='radio' className='appearance-none bg-transparent outline-none border-2 p-2.5 shadow-md'/>
              <p className='text-xl'>Remember Me</p>
            </div>
            <a href="#" className='ml-2 text-sellvora-textPrimary text-xl'><p>Forget password?</p></a>
          </div>
          <div id="submitbtn" className='w-full max-w-sm h-fit border-2 mt-5 text-2xl py-3 px-24 m-auto rounded-full mb-3 shadow-lg'>
            Submit
          </div>
          <p className='text-xl'>New to Sellvora? <a href="" className='text-sellvora-textPrimary text-xl'>Register.</a></p>
        </div>
    </>
  )
}
