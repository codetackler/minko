--
-- android_solution.lua
-- Generate a solution-level androidfile.
-- Copyright (c) 2012 Richard Swift and the Premake project
--

	function premake.jni_solution(sln)
		-- write a header showing the build options
		_p('# %s solution makefile autogenerated by Premake', premake.action.current().shortname)
		_p('APP_PROJECT_PATH := %s',sln.basedir)
		_p('APP_ABI := all')
		_p('')			
	end
	
