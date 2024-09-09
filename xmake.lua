add_rules("mode.debug", "mode.release")

target("CustomWindow")
    add_rules("qt.widgetapp")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")
    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("src/mainwindow.h")
    set_toolchains("msvc", {vs="2022"})
    add_cxxflags("/utf-8") 

    add_files("assets/image.qrc")

    set_policy("run.autobuild")