import React,{ useState } from 'react'

export default function customerlogin() {
    const[inputActive,setInputActive] = useState<'Email' | 'Password'>()
  return (
    <>
        <div id="loginform" className='mt-3 text-[#0f172a] flex-col justify-center m-auto text-center'>
          <h1 className='text-3xl m-auto'>Welcome Back</h1>
          <p>Welcome back! Enter your details</p>
          <div id="emaildiv" className='h-13 border-2 shadow-md w-fit m-auto rounded-md mt-4'>
              <p className={`${inputActive == 'Email' ? 'block' : 'hidden'} text-left pl-3`}>Enter your email.</p>
              <input type="email" id="email" className={`${inputActive == 'Email' ? 'mt-0' : 'mt-3' } bg-transparent px-4 outline-none`} placeholder={`${inputActive == 'Email' ? '' : 'Enter your email.'}`} onClick={() => setInputActive('Email')}/>
          </div>
          <div id="passdiv" className='h-13 border-2 shadow-md w-fit m-auto rounded-md mt-4'>
              <p className={`${inputActive == 'Password' ? 'block' : 'hidden'} text-left pl-3`}>Enter your password.</p>
              <input type="password" id="password" className={`${inputActive == 'Password' ? 'mt-0' : 'mt-3' } bg-transparent px-4 outline-none`} placeholder={`${inputActive == 'Password' ? '' : 'Enter your password.'}`} onClick={() => setInputActive('Password')}/>
          </div>
          <div id="action" className='mt-2 flex justify-center'>
            <input type='radio' className='appearance-none bg-transparent outline-none border-2 p-2 mr-1'/>
            <p>Remember Me</p>
            <a href="#" className='ml-2 text-blue-950'><p>Forget password?</p></a>
          </div>
          <div id="submitbtn" className='w-fit h-fit border-2 mt-2 text-2xl py-3 px-24 m-auto rounded-full'>
            Submit
          </div>
          <p>New to Sellvora?<a href="" className='text-blue-950'>Register.</a></p>
        </div>
    </>
  )
}
