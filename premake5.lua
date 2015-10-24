solution 'NativeDialog'
    location ( '.build' )
    flags {'C++11'}
    configurations { 'Debug', 'Release' }
    filter 'configurations:Debug'
        defines { 'DEBUG' }
        flags { 'Symbols' }
    filter 'configurations:Release'
        defines { 'NDEBUG' }
        optimize 'On'
    filter {}

    project 'Test'
        kind 'ConsoleApp'
        language 'C++'
        targetdir('.build/%{cfg.buildcfg}')
        includedirs {'src'}
        files { 'test.cc','src/**' }
        removefiles {"src/**.mm"}
        removefiles {'src/win/**'}
        if os.get() == 'linux' then
            buildoptions {'`pkg-config --cflags gtk+-3.0 glib-2.0`'}
            links {'gtk-3','glib-2.0','gobject-2.0'}
        elseif os.get() == 'macosx' then
          links {'Cocoa.framework'}
          files { 'src/osx/**' }
        end
