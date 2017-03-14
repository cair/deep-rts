for %%f in (*.fbs) do (
flatc.exe --cpp --python -b %%~nf.fbs
)
#move /Y *.h ../game/serialization/
#move /Y ./State ../game/python/
