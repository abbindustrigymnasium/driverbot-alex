<script lang="ts">
    // Importing necessary modules and styles
    import '../app.postcss'; 
    import { AppShell, AppBar } from '@skeletonlabs/skeleton'; 
</script>

<AppShell>
    <!-- Header section -->
    <svelte:fragment slot="header">
        <!-- App Bar component -->
        <AppBar>
           
            <svelte:fragment slot="lead">
              
                <div style="text-align: center;">
                 
                    <strong class="text-xl uppercase">Boberbot</strong>
                </div>
            </svelte:fragment>
            <!-- Trail section of the App Bar -->
            <svelte:fragment slot="trail">
                <!-- External links in the App Bar -->
                <a
                    class="btn btn-sm variant-ghost-surface"
                    href="https://baverjakt.se/k%C3%B6tt/"
                    target="_blank"
                    rel="noreferrer"
                >
                    Bobermeal
                </a>
                <a
                    class="btn btn-sm variant-ghost-surface"
                    href="https://www.youtube.com/watch?v=OHHpYXQyQO4"
                    target="_blank"
                    rel="noreferrer"
                >
                    OG-movie
                </a>
                <a
                    class="btn btn-sm variant-ghost-surface"
                    href="https://thumbs.dreamstime.com/z/north-american-beaver-closeup-snow-looks-like-smiling-64039944.jpg"
                    target="_blank"
                    rel="noreferrer"
                >
                    Smile
                </a>
            </svelte:fragment>
        </AppBar>
    </svelte:fragment>
    <!-- Page Route Content -->
    <slot /> 
</AppShell>

