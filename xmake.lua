add_rules("mode.debug", "mode.release")

target("CustomWindow")
    set_rules("qt.shared")

    add_headerfiles("src/*.h")
    add_files("src/*.cpp")
    add_includedirs("src", { public = true })

    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("src/CustomWindow.h")

    add_frameworks("QtGui", "QtWidgets")

    set_toolchains("msvc", {vs="2022"})
    add_cxxflags("/utf-8") 

    set_languages("cxx17")

    add_defines("CTW_EXPORT")

    add_files("assets/images/image.qrc")
