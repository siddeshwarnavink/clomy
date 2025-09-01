au BufNewFile,BufRead *.tmpl se ft=C
hi! link Error Normal

if has('win32') || has('win64')
  compiler msbuild
  set makeprg=msbuild\ /nologo\ /v:q\ /property:GenerateFullPaths=true\ build\clomy.sln
else
  compiler gcc
  set makeprg=make\ -C\ build/
endif
